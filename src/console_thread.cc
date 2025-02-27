#include "console_thread.h"
#include "threads/command_caller.h"

#include <iostream>
#include <tuple>

ConsoleThread::ConsoleThread(std::shared_ptr<Profiles> prof, std::shared_ptr<Processes> proc, std::shared_ptr<Logs> logs, std::shared_ptr<FileChooser> file_chooser)
: dispatch_man(std::move(prof), std::move(proc), std::move(logs), std::move(file_chooser))
{
  asynchronous_thread = std::async(std::launch::async, &ConsoleThread::console_caller, this);
}

void ConsoleThread::send_refresh_message(TabState new_state){
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  // Create a message with the state to refresh for, but no data
  Message message(REFRESH, new_state, {});
  // Send the message to the queue, this lets the other thread know what it should do.
  queue.push(message);
  cv.notify_one();
}

void ConsoleThread::send_change_profile_status_message(const std::string& profile, const std::string& old_status, const std::string& new_status){
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  // Create a message with the state to refresh for, but no data
  Message message(CHANGE_STATUS, OTHER, {profile, old_status, new_status});
  // Send the message to the queue, this lets the other thread know what it should do.
  queue.push(message);
  cv.notify_one();
}

void ConsoleThread::send_quit_message(){
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  Message message(QUIT, OTHER, {});
  queue.push(message);
  cv.notify_one();
}

void ConsoleThread::run_command(TabState state){
  switch (state)
  {
    case PROFILE:
    {
      std::string status = CommandCaller::get_status_str();
      dispatch_man.update_profiles(status);
    }
    break;

    case PROCESS:
    {
      std::string status = CommandCaller::get_status_str();
      std::string unconf = CommandCaller::get_unconfined();
      dispatch_man.update_processes(status, unconf);
    }
    break;

    case LOGS:
    {
      std::string logs = CommandCaller::get_logs_str();
      dispatch_man.update_logs(logs);
    }
    break;

    case FILECHOOSER:
    {
      std::string status = CommandCaller::get_status_str();
      dispatch_man.update_profiles(status);
    }
    break;


    case OTHER:
    // Do nothing.
    break;
  }
}

ConsoleThread::Message ConsoleThread::wait_for_message(){
  std::unique_lock<std::mutex> lock(task_ready_mtx);
  while(queue.empty()){
    cv.condition_variable::wait(lock); // Look into `wait_until`
  }

  return queue.pop();
}

void ConsoleThread::console_caller(){
  bool shouldContinue = true;
  while(shouldContinue) {
    Message message = wait_for_message();
    switch (message.event)
    {
      case REFRESH:
        run_command(message.state);
      break;

      case CHANGE_STATUS:
      {
        const std::string profile = message.data.at(0);
        const std::string old_status = message.data.at(1);
        const std::string new_status = message.data.at(2);
        std::string return_message = CommandCaller::execute_change(profile, old_status, new_status);
        dispatch_man.update_prof_apply_text(return_message);
        run_command(PROFILE);
      }
      break;

      case QUIT:
        shouldContinue = false;
      break;
    }
  }
}

// Move Assignment Operator
ConsoleThread& ConsoleThread::operator=(ConsoleThread&& other) noexcept{
  std::ignore = other;
  return *this;
}

ConsoleThread::~ConsoleThread(){
  send_quit_message();
  asynchronous_thread.wait();
}
