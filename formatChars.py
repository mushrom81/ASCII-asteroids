Fin = open("visualChars.txt","r")
chars = list(Fin.read())
Fin.close()

Fout = open("chars.txt","w")
for i in range(len(chars)):
    if (chars[i] == '\n'):
        if (chars[i + 1] != '0'):
            if (chars[i + 1] != '1'):
                Fout.write('\n')
    else:
        Fout.write(chars[i])
Fout.close()
