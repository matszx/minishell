#  **Minishell**

**Known issues:**
- [X] terminating child with a signal doesn't set exit status to the correct value
- [X] SIGQUIT not being handled in child processes
- [X] cat | cat | ls  and ctrl+c prints the promt message twice
- [X] empty inputs get added to history
- [X] echo "||" returns syntax error 
- [X] cd doesn't work with "-" argument
- [X] cd doesn't update the PWD and OLDPWD environment variables
- [X] execute looks into PATH when "./" path is specified
- [X] pwd in a deleted directory
- [X] norm

- Despite cd updates the environtment variables and the CWD variable, the amount of diferent cases (deleted directory, no PWD and/or OLDPWD, no HOME, - character) makes hard to understand the actual behaviour of the original command.
Therefore, for a more-like bash behaviour, a bigger amount of test is needed, although, it already works well enough