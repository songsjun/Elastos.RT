#==========================================================================
# Copyright (c) 2000-2014,  Elastos, Inc.  All Rights Reserved.
#==========================================================================

$(SPECIAL_TARGET):
	@echo Copying $(MAKEDIR)\dbus\dbus-1.dll to $(XDK_TARGETS) ...
	$(V) $(CP) $(MAKEDIR)\dbus\dbus-1.dll $(XDK_TARGETS) $(BLACKHOLE)
	@touch $@

