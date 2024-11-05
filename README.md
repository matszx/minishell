#  **Minishell**

**Known issues:**
- [X] terminating child with a signal doesn't set exit status to the corect value
- [X] SIGQUIT not being handled in child processes
- [X] empty inputs get added to history
- [ ] echo "||" returns syntax error 
- [ ] cd doesn´t work with "." argument
- [ ] execute looks into PATH when "./" path is specified
