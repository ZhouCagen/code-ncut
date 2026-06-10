#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
concat_files.py
将指定目录下的文件按“// 文件名 + 内容”的格式依次输出。

示例输出：
    // src/main.cpp
    #include <iostream>
    int main(){}

    // README.md
    ...

用法示例：
    python concat_files.py . --ext .c,.h,.cpp --recursive
    python concat_files.py ./notes --ext .md > all_notes.txt
"""

from __future__ import annotations
import argparse
from pathlib import Path
import sys

def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="按“// 文件名\\n<内容>”的格式将多个文件拼接输出"
    )
    p.add_argument(
        "root",
        nargs="?",
        default=".",
        help="扫描的根目录（默认：当前目录）"
    )
    p.add_argument(
        "--ext",
        required=True,
        help="要包含的文件后缀，逗号分隔，如：.c,.h,.cpp 或 py,md（大小写不敏感）"
    )
    p.add_argument(
        "-r", "--recursive",
        action="store_true",
        help="是否递归子目录（默认：否）"
    )
    p.add_argument(
        "--encoding",
        default="utf-8",
        help="读取文件所用编码（默认：utf-8；读取失败会自动以 errors='replace' 容错）"
    )
    p.add_argument(
        "--relative-to",
        default=None,
        help="用该目录作为相对前缀展示文件名（默认相对 root）"
    )
    return p.parse_args()

def norm_exts(ext_arg: str) -> set[str]:
    exts = set()
    for raw in ext_arg.split(","):
        s = raw.strip()
        if not s:
            continue
        if not s.startswith("."):
            s = "." + s
        exts.add(s.lower())
    return exts

def iter_files(root: Path, exts: set[str], recursive: bool):
    if recursive:
        yield from (p for p in root.rglob("*") if p.is_file() and p.suffix.lower() in exts)
    else:
        yield from (p for p in root.iterdir() if p.is_file() and p.suffix.lower() in exts)

def read_text_safely(p: Path, encoding: str) -> str:
    try:
        return p.read_text(encoding=encoding, errors="replace")
    except Exception:
        # 兜底：用二进制读再解码，尽量不给程序中断机会
        data = p.read_bytes()
        return data.decode(encoding, errors="replace")

def main():
    args = parse_args()
    root = Path(args.root).resolve()
    if not root.exists():
        print(f"[错误] 路径不存在：{root}", file=sys.stderr)
        sys.exit(1)

    exts = norm_exts(args.ext)
    base = Path(args.relative_to).resolve() if args.relative_to else root

    files = sorted(iter_files(root, exts, args.recursive), key=lambda p: str(p).lower())

    first = True
    for f in files:
        # 输出分隔头：// 相对路径
        rel = f.resolve().relative_to(base)
        header = f"// {rel.as_posix()}"
        content = read_text_safely(f, args.encoding)

        # 在文件块之间留一个空行；但开头不多加
        if not first:
            sys.stdout.write("\n")
        first = False

        sys.stdout.write(header + "\n")
        # 直接写原文；不额外添加尾随换行（保留原文件最后是否有换行）
        sys.stdout.write(content)

    # 若没有匹配，给个提示到 stderr
    if not files:
        print(f"[提示] 未找到后缀 {', '.join(sorted(exts))} 的文件（recursive={args.recursive}）", file=sys.stderr)

if __name__ == "__main__":
    main()
