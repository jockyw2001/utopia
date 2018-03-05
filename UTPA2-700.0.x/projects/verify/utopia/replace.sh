#!/bin/bash

for lib in `ls lib/*.so`
do
	echo replace $lib
	cp $lib /vendor/lib/utopia/
done

