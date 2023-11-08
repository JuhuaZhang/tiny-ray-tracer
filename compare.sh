#!/bin/zsh

# compare images
student_file=$1
ref_file="mprayfiles/$student_file"

compare -fuzz 2% "$student_file" "$ref_file" ae.png
composite "$student_file" "$ref_file" -alpha off -compose difference rawdiff.png
convert rawdiff.png -level 0%,8% diff.png
convert +append "$ref_file" "$student_file" ae.png rawdiff.png diff.png "${student_file%.*}_result.png"

rm $student_file ae.png rawdiff.png diff.png
