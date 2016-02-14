
all = change_apk_icon

${all}: change_apk_icon.o
	gcc -o change_apk_icon change_apk_icon.o -g
	mv change_apk_icon ./out/change_apk_icon


change_apk_icon.o : change_apk_icon.c
	gcc -c change_apk_icon.c -g



clean : ${all} change_apk_icon.o
	-rm ${all} change_apk_icon.o
