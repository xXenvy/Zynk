#include <gtest/gtest.h>

#include "../src/common/include/cli.hpp"
#include "../src/common/include/errors.hpp"

TEST(CLIArgsTest, FileArgumentShortOption) {
    CLI cli({ "main.zk" });
    EXPECT_TRUE(!cli.args.file_path.empty());
}

TEST(CLIArgsTest, FileArgumentLongOption) {
    CLI cli({ "--file_path main.zk" });
    EXPECT_TRUE(!cli.args.file_path.empty());
}

TEST(CLIArgsTest, HelpArgumentShortOption) {
    CLI cli({ "help" });
    EXPECT_TRUE(cli.args.help);
}

TEST(CLIArgsTest, HelpArgumentLongOption) {
    CLI cli({ "--help" });
    EXPECT_TRUE(cli.args.help);
}

TEST(CLIArgsTest, VersionArgumentShortOption) {
    CLI cli({ "version" });
    EXPECT_TRUE(cli.args.version);
}

TEST(CLIArgsTest, VersionArgumentLongOption) {
    CLI cli({ "--version" });
    EXPECT_TRUE(cli.args.version);
}

TEST(CLIArgsTest, InitArgumentShortOption) {
    CLI cli({ "init" });
    EXPECT_TRUE(cli.args.init);
}

TEST(CLIArgsTest, InitArgumentLongOption) {
    CLI cli({ "--init" });
    EXPECT_TRUE(cli.args.init);
}

TEST(CLIArgsTest, ShouldBeEmpty) {
    CLI cli({});
    EXPECT_TRUE(cli.args.empty());
}

TEST(CLICheckoutTest, ShouldThrowNoArgumentWasGiven) {
    CLI cli({});
    EXPECT_THROW(cli.checkout(), ZynkError);
}

TEST(CLICheckoutTest, ShouldThrowTooManyArguments) {
    CLI cli({"--file_path main.zk", "--help"});
    EXPECT_THROW(cli.checkout(), ZynkError);
}

TEST(CLIArguments, ShouldBeEmpty) {
    Arguments args(0);
    EXPECT_TRUE(args.empty());
}

TEST(CLIArguments, ShouldNotBeEmpty) {
    Arguments args(3, "main.zk", true, true);

    EXPECT_FALSE(args.empty());
    EXPECT_TRUE(args.help);
    EXPECT_TRUE(args.version);
    EXPECT_FALSE(args.init);
    EXPECT_FALSE(args.file_path.empty());
}