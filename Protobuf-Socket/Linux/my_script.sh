#!/bin/bash

function main(){

        check "$@"
}

function check(){
        local options
        File="my_file.txt"
        while getopts n:p:o:t: options; do
                case $options in
                        n) name="$OPTARG";;
                        p) phone="$OPTARG";;
                        o) one="$OPTARG";;
                        t) two="$OPTARG";;

                esac
        done

# Writing to file
        if [ ! -e "$File" ]; then
            touch $File
        fi

        if [ "$name" = "" ]
        then
                # echo "Name: No Name"
                echo "Name: No Name" > $File
        else
                # echo "Name: $name"
                echo "Name: $name" > $File
        fi

        if [ "$phone" = "" ]
        then
                # echo "Phone: No Phone"
                echo "Phone: No Phone" >> $File
        else
                # echo "Phone: $phone"
                echo "Phone: $phone" >> $File
        fi

        if [ "$one" = "" ]
        then
                # echo "Note1: blank"
                echo "Note1: blank" >> $File
        else
                # echo "Note1: $one"
                echo "Note1: $one" >> $File
        fi

        if [ "$two" = "" ]
        then
                # echo "Note2: blank"
                echo "Note2: blank" >> $File
        else
                # echo "Note2: $two"
                echo "Note2: $two" >> $File
        fi

    wc -c < $File

}


main "$@"