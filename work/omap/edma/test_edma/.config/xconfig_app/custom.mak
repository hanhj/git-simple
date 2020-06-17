## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,674 linker.cmd \
  package/cfg/app_p674.o674 \

linker.cmd: package/cfg/app_p674.xdl
	$(SED) 's"^\"\(package/cfg/app_p674cfg.cmd\)\"$""\"D:/workspace/ccs5/test_edma/.config/xconfig_app/\1\""' package/cfg/app_p674.xdl > $@
