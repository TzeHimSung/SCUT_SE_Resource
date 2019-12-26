#!/bin/sh -e

cd "$(dirname "$0")"

n=3
rm -f eg.*.*

add() {
    echo "$*" >>eg.$2.$3
}

add 0.1 0 0 2 1 1
add 0.2 1 0 1 2 1

cat eg.*.* >eg
