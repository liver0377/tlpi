SOURCE=$(find -name "*.c")

for i in ${SOURCE[@]}; do  \
    gcc $i -o ${i%.c}.o -ltlpi -L.
done