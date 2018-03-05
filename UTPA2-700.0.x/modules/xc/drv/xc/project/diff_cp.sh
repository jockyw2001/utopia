if [ ! -f $1 ]
then
    exit 0
fi

diff $1 $2 > /dev/null
if [ $? -ne 0 ]
then 
    cp -f $1 $2
fi
