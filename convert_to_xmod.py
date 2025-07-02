#!/usr/bin/env python3
import sys
import os
import json
import zipfile


def make_manifest(name, version, description, dependencies):
    """
    Формирует Python-словарь для manifest.json
    """
    return {
        "name": name,
        "version": version,
        "description": description,
        "dependencies": dependencies
    }


def create_xmod(bin_path, name, version, description, dependencies, output_dir="."):
    """
    Создаёт файл .xmod на основе бинарника и метаданных
    """
    if not os.path.isfile(bin_path):
        print(f"Ошибка: файл {bin_path} не найден.")
        return False

    manifest = make_manifest(name, version, description, dependencies)
    base = os.path.basename(bin_path)
    mod_name, _ = os.path.splitext(base)
    xmod_name = f"{mod_name}.xmod"
    xmod_path = os.path.join(output_dir, xmod_name)

    with zipfile.ZipFile(xmod_path, 'w', zipfile.ZIP_DEFLATED) as zf:
        # добавляем бинарник
        zf.write(bin_path, arcname=base)
        # добавляем manifest.json
        manifest_bytes = json.dumps(manifest, indent=2).encode('utf-8')
        zf.writestr('manifest.json', manifest_bytes)

    print(f"Создан модуль: {xmod_path}")
    return True


if __name__ == '__main__':
    if len(sys.argv) < 6:
        print("Использование: convert_to_xmod.py <bin_path> <name> <version> <description> <dependencies(comma-separated)>")
        sys.exit(1)
    bin_path = sys.argv[1]
    name = sys.argv[2]
    version = sys.argv[3]
    description = sys.argv[4]
    deps = sys.argv[5].split(',') if sys.argv[5] else []

    create_xmod(bin_path, name, version, description, deps)