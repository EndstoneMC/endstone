"""
Headless IDA auto-analysis driver. Run via:

    idat.exe -A -c -L<log> -S"make_idb.py" <binary>

`-c` creates a fresh database (deleting any existing one), `-A` runs
autonomously (no dialogs). IDA loads the binary, then runs this script: we block
until auto-analysis finishes, save the database, and quit. The msg() lines land
in the `-L` log so progress is visible while the job runs in the background.

This produces a clean `.i64` with no `.asm` byproduct (unlike `idat -B`).
"""

import ida_auto
import ida_kernwin
import ida_pro
import idc

ida_kernwin.msg("[make_idb] load complete; waiting for auto-analysis...\n")
ida_auto.auto_wait()  # blocks until the analysis queue drains

idb = idc.get_idb_path()
ida_kernwin.msg(f"[make_idb] analysis done; saving {idb}\n")
idc.save_database(idb, 0)

ida_kernwin.msg("[make_idb] saved; exiting.\n")
ida_pro.qexit(0)
