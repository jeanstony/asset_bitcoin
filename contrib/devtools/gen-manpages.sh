#!/usr/bin/env bash

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

VCCOIND=${VCCOIND:-$BINDIR/VCcoind}
VCCOINCLI=${VCCOINCLI:-$BINDIR/VCcoin-cli}
VCCOINTX=${VCCOINTX:-$BINDIR/VCcoin-tx}
WALLET_TOOL=${WALLET_TOOL:-$BINDIR/VCcoin-wallet}
VCCOINQT=${VCCOINQT:-$BINDIR/qt/VCcoin-qt}

[ ! -x $VCCOIND ] && echo "$VCCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little VC
read -r -a BTCVER <<< "$($VCCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }')"

# Create a footer file with copyright content.
# This gets autodetected fine for VCcoind if --version-string is not set,
# but has different outcomes for VCcoin-qt and VCcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$VCCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $VCCOIND $VCCOINCLI $VCCOINTX $WALLET_TOOL $VCCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
