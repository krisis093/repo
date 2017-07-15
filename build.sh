#init directory
mkdir -p builds/boot

# C flags
CFLAGS="-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c"
gcc $CFLAGS src/kernel.c -o builds/kernel.o

#assembly and link the file...
nasm -f elf32 src/boot.asm -o builds/boot.o
nasm -f elf32 src/rootUtils.asm -o builds/rootUtils.o

# link all files
ld -T linkScript.ld -melf_i386 builds/rootUtils.o builds/kernel.o builds/boot.o -o builds/boot/os.elf

# get GRUB to load the os
mkdir -p builds/boot/grub
cp thirdparty/stage2_eltorito builds/boot/grub

# set grub
touch builds/boot/grub/menu.lst
echo "default=0\ntimeout=0\n\ntitle myOS\nkernel /boot/os.elf\n" > builds/boot/grub/menu.lst

#burn ISO
genisoimage -R                              \
            -b boot/grub/stage2_eltorito    \
            -no-emul-boot                   \
            -boot-load-size 4               \
            -A os                           \
            -input-charset utf8             \
            -quiet                          \
            -boot-info-table                \
            -o os.iso                       \
            builds

# remove partial builds
rm -rf builds
