## Step1: Run egos-2000 on the QEMU emulator

```shell
> make qemu
-------- Simulate on QEMU-RISCV --------
cp build/release/earth.elf tools/qemu/qemu.elf
riscv64-unknown-elf-objcopy --update-section .image=tools/disk.img tools/qemu/qemu.elf
qemu-system-riscv32 -readconfig tools/qemu/sifive-e31.cfg -kernel tools/qemu/qemu.elf -nographic
[INFO] -----------------------------------
[INFO] Start to initialize the earth layer
......
```

## Step2: Quitting the QEMU emulator
1. Type ctrl-a then press x
