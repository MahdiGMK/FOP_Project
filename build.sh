for D in src/*; do
    if [ -d "${D}" ] && [ "${D}" != "src/shared" ]; then
        gcc "$D/"*.c "src/shared/"*.c -I "src/" -lm -lncursesw -o "bin/$(basename $D).out"
    fi
done
