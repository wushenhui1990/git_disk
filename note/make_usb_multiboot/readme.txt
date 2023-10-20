先用老毛桃初始化安装U盘，再格式化前面的disk，安装grub
分区1 exfat
分区2 fat32 50m
分区3 老毛桃

mount exfat to exfat
mount fat32 to boot
sudo grub-install --target=i386-pc --boot-directory=boot /dev/sdc
sudo grub-install --removable --target=x86_64-efi --boot-directory=exfat/boot --efi-directory=boot
sudo grub-install --removable --target=i386-efi --boot-directory=exfat/boot --efi-directory=boot

