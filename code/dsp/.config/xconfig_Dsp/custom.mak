## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e674 linker.cmd \
  package/cfg/Dsp_pe674.oe674 \

linker.cmd: package/cfg/Dsp_pe674.xdl
	$(SED) 's"^\"\(package/cfg/Dsp_pe674cfg.cmd\)\"$""\"E:/cc52workplace/DSPCPP/.config/xconfig_Dsp/\1\""' package/cfg/Dsp_pe674.xdl > $@
