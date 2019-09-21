
# Contributing To AtCore
AtCore is an open source project and all open source projects need contrubitors. Thank you for taking the time to read this document and Thank you for your future contributions to this project. 

----
## Prerequisites
In order to contrbute directly to AtCore you will need to have a [KDE Identity]. You should create a fork on [KDE Invent] to work within. You may wish to join us on [IRC] or [Telegram].

## Ways you can contribute
There are several ways you can contribute
 - Submit or Help to confirm bugs in our [Bug Tracker].
 - Find Errors in the documentation or help to otherwise improve it
 - Help users solve their issues
 - Help to review Code (Watch the project on [KDE Invent])
 - Commit new code or code to fix bugs. 

## General Contribution Workflow
The Following Is the General work flow for most contrubitions
 - Create changes on your local fork in a new branch
 - When your ready to submit your change create a Merge Request on [KDE Invent]
 - Wait for review
 - Update your commits with any changes, and update your Merge Request after.
 - Mark all your "Issues" resolved in your MR. 
 - Repeat Review / Update until changes is accepted.
 - A Maintainer will Merge when the changes is ready.
 
### General Coding Guidelines
The following guidelines apply to all submited code.

 - Follow the [KDE style guidelines]
 - Use constant referances when possible
 - Includes should be alphabetical.
 - Run the style script before you commit.
 - Headers should use `#pragma once`.
 - Auto should not be used if the type is not explict.
 - Doxygen comments should be created/updated with code.
 - Objects should have logical varible and function names
 - Code should always be readable, comment your code when it makes sense. 

### Commit Guidelines
Keeping a clean project history requires that we take care when choosing how we will commit our code. Consider the following when creating commits. ALL COMMITS MUST BE REVIEWED BEFORE PUSHING TO MASTER.

 - Follow the [KDE commit guidelines]
 - Reviewers: rizzitello, patrickelectric, laysrodrigues, tcanabrava
 - Be sure to make any review issues as resolved.
 - Keep your base upto date. 
 - Amend your local commits to fix review issues.
 - Always Ammend commits to update them 
 - Never fix a commit with another commit.
 - Update your commits with `git commit --amend`
 - Update your MR with `git push --force`
 - Revisions require a majority acceptance rate before landing
 - Your Merge Request will be merged when your code when its ready

[IRC]: https://webchat.freenode.net/
[Telegram]: https://telegram.me/KDEAtelier
[Bug Tracker]: https://bugs.kde.org/enter_bug.cgi?product=Atelier&component=AtCore
[KDE Identity]: https://identity.kde.org/
[KDE Invent]: https://invent.kde.org
[KDE style guidelines]: https://community.kde.org/Policies/Kdelibs_Coding_Style
[KDE commit guidelines]: https://community.kde.org/Policies/Commit_Policy
