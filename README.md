#  **Minishell**

**Known issues:**
- [X] terminating child with a signal doesn't set exit status to the correct value
- [X] SIGQUIT not being handled in child processes
- [X] cat | cat | ls  and ctrl+c prints the promt message twice
- [X] empty inputs get added to history
- [X] echo "||" returns syntax error 
- [ ] cd doesn't work with "." argument
- [ ] cd doesn't update the pwd environtment variable
- [X] execute looks into PATH when "./" path is specified
- [ ] pwd in a deleted directory
- [ ] norm