if [[ -f ./setup_cache.tmp ]]; then
    read -p "Would you like to recover the previous session?(y/n)" bool
    if [[ $bool == y ]]; then
        {
            read -r currdir
            read -r currfile
            read -r ext
            read -r comp
            read -r exec
            read -r run
        } < "./setup_cache.tmp"

        export currdir
        export currfile
        export ext
        export comp
        export exec
        export run

        return 0 2>/dev/null || exit 0
    fi
fi

read -p "Work with C or C++ file? (0/1)" ext
if [[ $ext == 0 ]]; then ext=c; compiler=gcc; else ext=cpp; compiler=g++; fi

read -p "Which directory to work with?" currdir
currdir="$(pwd)/$currdir"

read -p "Which file to work with?" currfile

comp="$compiler \"$currdir/$currfile.$ext\" -o \"$currdir/$currfile.out\""
exec="\"$currdir/$currfile.out\""
run="eval \$comp && eval \$exec"

printf "%s\n" "$currdir" "$currfile" "$ext" "$comp" "$exec" "$run" > "./setup_cache.tmp"

export currdir
export currfile
export ext
export comp
export exec
export run
