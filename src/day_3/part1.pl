#!/usr/bin/perl
use strict;

# part 1: sum of all mul(x, y) instructions in the text
my $data = <<'END_TXT';
redacted...
source: https://adventofcode.com/2024/day/3/input
END_TXT

my @matches = $data =~ /mul\(\d+,\d+\)/g;

my $sum = 0;
foreach (@matches) {
    my @numbers = $_ =~ /\d+/g;
    $sum += @numbers[0] * @numbers[1];
}
print($sum)
