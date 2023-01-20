for D in src/*; do
    if [ -d "${D}" ] && [ "${D}" != "src/shared" ]; then
        if [ -e "src/shared/"*.c ]; then
            gcc "$D/"*.c "src/shared/"*.c -I "src/" -lm -lncurses -o "bin/$(basename $D).out"
        else
            gcc "$D/"*.c -I "src/" -lm -lncurses -o "bin/$(basename $D).out"
        fi
    fi
done
