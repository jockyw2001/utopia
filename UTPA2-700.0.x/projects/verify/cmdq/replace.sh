#!/bin/bash

for lib in `ls *.so`
do
	echo replace $lib
	cp $lib /vendor/lib/utopia/
done

