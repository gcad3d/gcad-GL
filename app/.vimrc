let tagfiles = system("ls -m ../tags/*.tag |sed \"s/ //g\"")
"echo tagfiles
let &tags = substitute(tagfiles, "\n", "", "g")

" EOF
