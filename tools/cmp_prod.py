#!/usr/bin/env python3
"""Structurally compare two .prod listings, ignoring alternative order.

ParserMaker does not emit alternatives in .pmk source order, so a byte diff is
meaningless. This compares, per rule, the *multiset* of alternatives.
"""
import re
import sys
from collections import OrderedDict


def load(path):
    """-> OrderedDict rule name -> list of alternative strings"""
    rules, cur = OrderedDict(), None
    for line in open(path):
        line = line.rstrip()
        if not line.strip() or line.strip() == ";":
            continue
        # "  1. <adventure> = rhs"  |  " 15.        ! rhs"
        m = re.match(r"\s*(\d+)\.\s*(.*)$", line)
        if not m:
            # Continuation line: the listing wraps long productions onto
            # unnumbered lines. Append to the alternative still being read.
            if cur and rules.get(cur):
                rules[cur][-1] += " " + line.strip()
            continue
        body = m.group(2)
        if "=" in body and not body.lstrip().startswith("!"):
            head, rhs = body.split("=", 1)
            cur = head.strip()
            rules.setdefault(cur, []).append(rhs.strip())
        elif body.lstrip().startswith("!"):
            rules[cur].append(body.lstrip()[1:].strip())
    return rules


def norm(alt):
    return re.sub(r"\s+", " ", alt).strip()


def main():
    a, b = load(sys.argv[1]), load(sys.argv[2])
    na, nb = sys.argv[1], sys.argv[2]

    ka, kb = set(a), set(b)
    only_a, only_b = sorted(ka - kb), sorted(kb - ka)

    print(f"rules: {na}={len(a)}  {nb}={len(b)}")
    prod_a = sum(len(v) for v in a.values())
    prod_b = sum(len(v) for v in b.values())
    print(f"productions: {na}={prod_a}  {nb}={prod_b}")
    print()

    if only_a:
        print(f"rules only in {na} ({len(only_a)}): {', '.join(only_a)}\n")
    if only_b:
        print(f"rules only in {nb} ({len(only_b)}): {', '.join(only_b)}\n")

    same_set, diff_order, differ = 0, 0, []
    for k in a:
        if k not in b:
            continue
        A = [norm(x) for x in a[k]]
        B = [norm(x) for x in b[k]]
        if A == B:
            same_set += 1
        elif sorted(A) == sorted(B):
            same_set += 1
            diff_order += 1
        else:
            differ.append((k, A, B))

    print(f"rules with identical alternative multiset: {same_set}"
          f"  (of which {diff_order} differ only in ORDER)")
    print(f"rules that genuinely differ: {len(differ)}")
    print()
    for k, A, B in differ:
        print(f"--- {k}")
        onlyA = [x for x in A if x not in B]
        onlyB = [x for x in B if x not in A]
        for x in onlyA:
            print(f"    only {na}: {x}")
        for x in onlyB:
            print(f"    only {nb}: {x}")


if __name__ == "__main__":
    main()
