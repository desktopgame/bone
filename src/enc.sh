find . -iname *.h | xargs nkf --overwrite --oc=UTF-8-BOM
find . -iname *.c | xargs nkf --overwrite --oc=UTF-8-BOM