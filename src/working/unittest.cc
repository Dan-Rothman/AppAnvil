#include <limits.h>
#include "command_caller.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "../tabs/status.h"


/*class testStatus : public Status::Status, public testing::Test
{
   // Empty - bridge to protected members for unit-testing
		bool wrapfilter(const std::string& str, const std::string& rule, const bool& use_regex, const bool& match_case, const bool& whole_word){
			return Status::filter(str, rule, use_regex, match_case, whole_word);
		}
};*/


TEST(COMMAND_CALLER, GET_STATUS_STR){
	std::string rt = CommandCaller::get_status_str();
	EXPECT_THAT(rt, testing::HasSubstr("\"version\": \"1\""));
}


TEST(COMMAND_CALLER, GET_UNCONFINED){
	std::string rt = CommandCaller::get_unconfined();
	EXPECT_THAT(rt, testing::HasSubstr("not confined"));
}


TEST(COMMAND_CALLER, GET_LOGS_STR){
	std::string rt = CommandCaller::get_logs_str();
	EXPECT_THAT(rt, testing::HasSubstr("Linux version"));
}


TEST(STATUS, FILTER_FFF){
	bool res = Status::filter("abcdEFGH", "cDEf", false, false, false);
	//bool res = testStatus::wrapfilter("abcdEFGH", "cDEf", false, false, false);
	EXPECT_TRUE(res);

	res = Status::filter("abcdEFGH", "gHj", false, false, false);
	EXPECT_FALSE(res);
}


TEST(STATUS, FILTER_TFF){
	bool res = Status::filter("abcdEFGH", "c*f", true, false, false);
	EXPECT_TRUE(res);

	res = Status::filter("abcdEFGH", "g*j", true, false, false);
	EXPECT_FALSE(res);
}


TEST(STATUS, FILTER_FTF){
	bool res = Status::filter("abcdEFGH", "cdEF", false, true, false);
	EXPECT_TRUE(res);

	res = Status::filter("abcdEFGH", "cdef", false, true, false);
	EXPECT_FALSE(res);
}


TEST(STATUS, FILTER_FFT){
	bool res = Status::filter("abcdEFGH", "abcdEFGh", false, false, true);
	EXPECT_TRUE(res);

	res = Status::filter("abcdEFGH", "abcdEFG", false, false, true);
	EXPECT_FALSE(res);
}

/*TEST(STATUS, FILTER_FFF){
	bool res = Status::filter("abcdefg", "ABC", false, false, false);
	//bool res = testStatus::wrapfilter("abcdefg", "ABC", false, false, false);
	EXPECT_TRUE(res);
}*/



int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}