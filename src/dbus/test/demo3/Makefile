





interface_xml := value-dbus-interface.xml

checkxml: $(interface_xml)
	echo $< check out ok
	xmllint --valid --noout $(interface_xml)

files := client-glub.h\
	 server-glub.h

value-client-glub.h: $(interface_xml)
	dbus-binding-tool --prefix=value_object --mode=glib-client  $< > $@


value-server-glub.h: $(interface_xml)
	dbus-binding-tool --prefix=value_object --mode=glib-server  $< > $@
