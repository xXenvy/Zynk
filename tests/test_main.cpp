#include <gtest/gtest.h>
#include "../src/cli.h"

TEST(CLITest, HelpArgument) {
    const std::vector<std::string> args = { "help" };
    CLI cli(args);
    EXPECT_TRUE(cli.args.help);
}