# simple encryption

import os
from cryptography.fernet import Fernet

def generate_key():
    return Fernet.generate_key()

def save_key(key):
    with open("encryption_key.key", "wb") as key_file:
        key_file.write(key)

def load_key():
    return open("encryption_key.key", "rb").read()

def encrypt_file(file_path, key):
    fernet = Fernet(key)
    with open(file_path, "rb") as file:
        original = file.read()
    encrypted = fernet.encrypt(original)
    with open(file_path, "wb") as encrypted_file:
        encrypted_file.write(encrypted)

def read_bootloader():
    with open("bootloader.asm", "rb") as f:
        return f.read()

def read_kernel():
    with open("kernel.c", "rb") as f:
        return f.read()

def write_bootloader():
    bootloader_code = read_bootloader()
    with open("bootloader.bin", "wb") as f:
        f.write(bootloader_code)
    os.system("nasm -f bin bootloader.asm -o bootloader.bin")

def write_kernel():
    kernel_code = read_kernel()
    with open("kernel.c", "wb") as f:
        f.write(kernel_code)
    os.system("gcc -ffreestanding -c kernel.c -o kernel.o")
    os.system("ld -T linker.ld -o kernel.bin kernel.o")
    os.system("cat bootloader.bin kernel.bin > os-image.bin")

if __name__ == "__main__":
    key = generate_key()
    save_key(key)

    write_bootloader()
    write_kernel()

    encrypt_file("bootloader.bin", key)
    encrypt_file("kernel.bin", key)
