# Render the erroring source line with any carriage return made visible.
#
# Jregr normalises line endings before it compares, so a stray CR at the end
# of a listed source line does not show up in a plain .expected at all. This
# turns it into text, which does. Written as an awk script rather than a sed
# 'l' command because BSD and GNU sed disagree about what 'l' prints.
/sadf/ { gsub(/\r/, "<CR>"); print }
