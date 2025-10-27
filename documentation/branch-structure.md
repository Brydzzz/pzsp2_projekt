# Branch structure

## main
Has the newest working code from all the dev branches. 

## main-copy
This is the branch that has the code from main branch rolled back by one commit (in case of some disaster). That means that each pull request should be preceded by a merge from main to main-copy.

## *-dev
These are the branches used by developers to edit code before pushing to main, or to feature.

## *-feature
These are the branches that make a specific feature. Multiple devs can take code from and push code to a feature branch.