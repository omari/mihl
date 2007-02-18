set confirm off
file ./example4
handle SIGINT  stop pass
handle SIGUSR1 nostop pass
handle SIGUSR2 nostop pass
handle SIG32 nostop pass
break exit
break main
run
