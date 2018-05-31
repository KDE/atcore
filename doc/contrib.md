
# Contributing To AtCore
%AtCore is an open source project and all open source projects need contrubitors. Thank you for taking the time to read this document and Thank you for your contributions to the project. 

----
## Prerequisites
In order to contrbute directly to %AtCore you will need to have a [KDE Identity]. You should also install [arcanist] on your system since it will make creating revisions for 
code review much easier.You may wish to join us on [IRC] or [Telegram]


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
 - Code should alway be readable ,comment your code.

### Commit Guidelines
Keeping a clean project history requires that we take care when choosing how we will commit our code.Consider the following when creating commits. ALL COMMITS MUST BE REVIEWED BEFORE PUSHING TO MASTER.

 - Follow the [KDE commit guidelines]
 - Reviewers: rizzitello, patrickelectric, laysrodrigues, tcanabrava
 - Be sure to make things "done" in phabricator.
 - Update your revisions with `arc diff --update <diff number>`
 - Amend your local commits to fix review issues.
 - Revisions require a majority acceptance rate before landing
 - Land your code with `arc land <branch>`
 
[IRC]: https://webchat.freenode.net/
[Telegram]: telegram.me/KDEAtelier
[KDE Identity]: https://identity.kde.org/
[arcanist]:https://secure.phabricator.com/book/phabricator/article/arcanist/
[KDE style guidelines]:https://community.kde.org/Policies/Kdelibs_Coding_Style
[KDE commit guidelines]:https://community.kde.org/Policies/Commit_Policy
