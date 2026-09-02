# 'install' rather than 'cp': the copy from a previous run is still
# there, and overwriting it in place would keep the inode and so the
# code signature macOS has cached for it, making the renamed arun die
# with SIGKILL. See the INSTALL comment in mk/common.mk.
install -c ../../bin/arun rename_as_game
cp ../saviour.a3c rename_as_game.a3c
./rename_as_game -r -n < rename_as_game.input
