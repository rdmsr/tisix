LIMINE=thirdparty/limine/limine-install
SYSROOT=$(BUILDDIR)/sysroot
ISO=$(BUILDDIR)/sysroot.iso

$(LIMINE):
	make -C thirdparty/limine all

$(SYSROOT): $(SERVERS) sysroot/boot/limine.cfg $(KERNEL)
	$(DIR_GUARD)
	rm -rf $(SYSROOT)
	cp -R sysroot                                $(SYSROOT)

	cp $(SERVERS) $(SYSROOT)/boot
	cp $(KERNEL)                                 $(SYSROOT)/boot/kernel.elf
	cp thirdparty/limine/limine.sys              $(SYSROOT)/boot/limine.sys
	cp thirdparty/limine/limine-cd.bin           $(SYSROOT)/boot/limine-cd.bin
	cp thirdparty/limine/limine-eltorito-efi.bin $(SYSROOT)/boot/limine-eltorito-efi.bin

$(ISO): $(SYSROOT) $(LIMINE)
	$(DIR_GUARD)

	xorriso \
		-as mkisofs \
		-b boot/limine-cd.bin \
		-no-emul-boot \
		-boot-load-size $(shell nproc --all) \
		-boot-info-table \
		--efi-boot boot/limine-eltorito-efi.bin \
		-efi-boot-part \
		--efi-boot-image \
		--protective-msdos-label \
		$(SYSROOT) \
		-o $(ISO)

	$(LIMINE) $(ISO)