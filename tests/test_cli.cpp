#include <gtest/gtest.h>
#include "../src/cli.h"

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
    EXPECT_THROW(cli.checkout(), std::logic_error);
}

TEST(CLICheckoutTest, ShouldThrowTooManyArguments) {
    CLI cli({"--file_path main.zk", "--help"});
    EXPECT_THROW(cli.checkout(), std::logic_error);
}

TEST(CLIArguments, ShouldBeEmpty) {
    Arguments args{};
    EXPECT_TRUE(args.empty());
}

TEST(CLIArguments, ShouldNotBeEmpty) {
    Arguments args{"main.zk", true, true, false};
    EXPECT_FALSE(args.empty());

    EXPECT_TRUE(args.help);
    EXPECT_TRUE(args.version);
    EXPECT_FALSE(args.init);
    EXPECT_FALSE(args.file_path.empty());
}