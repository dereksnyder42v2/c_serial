#!/usr/bin/python3

import os

if __name__ == "__main__":
    outfile = open("headcheck_test.c", "w")
    header_buff = []
    keywords = []
    
    print("Write all potential headers, pressing RETURN after each. Press ^C to finish. (Exclude stdio.h)")
    while 1:
        try:
            header_buff.append(input("> ") )
        except KeyboardInterrupt:
            break
    outfile.write("#include <stdio.h>\n")
    for header in header_buff:
        outfile.write("#include <%s>\n" % header)
    outfile.write(
"""
int main(void)
{

""")
    print("\nWrite all keywords of interest, pressing RETURN after each. Press ^C to finish.")
    while 1:
        try:
            keywords.append(input("> ") )
        except KeyboardInterrupt:
            break
    for keyword in keywords:
        outfile.write(
"""
#ifdef %s
    printf("%s found.\\n");
#endif
""" % (keyword, keyword)
)

    outfile.write(
"""
    return 0;
}
""")
    outfile.close()
    os.system("cat headcheck_test.c")
    """
    looksgood = input("OK? [y/n]")
    if looksgood != "y":
        quit()
    else:
        pass
    """

    print()
    os.system("gcc headcheck_test.c -o Headcheck_test && ./Headcheck_test")
    print()
    os.system("rm headcheck_test.c")
    os.system("rm Headcheck_test")
    print()
