# socketTCP
 written in C, works in linux terminal

clientStruct/serverStruct = Built with struct, it will give a copied file with the proper file size, but each buffer has 4 letters cut off the end for some reason. I even had my professor look at it and he couldn't find out what was wrong with it either.

clientNoStruct/serverNoStruct = Will give you the proper characters but it has null pointers at the end and the file size is wrong. when using "diff" it will give you the result 1c1, meaning it is the same but there is empty space in one of the files.

