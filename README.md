# blogc-git-receiver

A simple login shell/git hook to deploy blogc websites.

`blogc-git-receiver` provides a PaaS-like way to deploy your blogc websites. When used as a login shell, it will accept git payloads, creating bare repositories as needed, and installing a hook, that will take care of building your website each time you push something to the `master` branch.

The git repository must provide a `Makefile` (or a `GNUMakefile`), that should accept the `OUTPUT_DIR` variable, and install built files in the directory pointed by this variable.

`blogc-git-receiver` is part of `blogc` project, but isn't tied to `blogc`. Any repository with `Makefile` that builds content and install it to `OUTPUT_DIR` should works with `blogc-git-receiver`.

## Example setup

Download a [release tarball](https://github.com/blogc/blogc-git-receiver/releases), extract it and enter its directory, after that run the following commands:

    $ ./configure
    $ make
    # make install

If you ran `./configure` without `--prefix`, your binary will be installed to `/usr/local/bin/blogc-git-receiver`. Now you must create a new user, using this shell.

    # useradd -m -s /usr/local/bin/blogc-git-receiver blogc

Users installing `blogc-git-receiver` from distribution packages must use the following command, because the binary is installed directly in `/usr/bin`:

    # useradd -m -s /usr/bin/blogc-git-receiver blogc

Now you can add your ssh keys to `/home/blogc/.ssh/authorized_keys`. Also, make sure to install all the dependencies required by your websites, including a web server. `blogc-git-receiver` won't handle your web server virtual hosts, at least for now.

To deploy a website (e.g. our example repository):

    $ git clone https://github.com/blogc/blogc-example.git
    $ cd blogc-example
    $ git remote add blogc blogc@${SERVER_IP}:blogs/blogc-example.git
    $ git push blogc master

This will deploy the example to the remote server, creating a symlink to your built content in `/home/blogc/repos/blogs/blogc-example.git/htdocs`, you just need to point the web server to this symlink, configure your rewrite rules, if needed, and you're done.

## Repository mirroring

Users can rely on `blogc-git-receiver` to mirror their repositories to a remote Git repository (e.g. a free Bitbucket private repository). This feature just requires a `.mirror` file in the root of the repository, with the git URL of the remote repository. Please note that the `blogc` user must be able to push to the remote repository, and that any content manually pushed to the remote repository is overwritten by `blogc-git-receiver`, because it will `git push --mirror --force` to the remote repository.

Some reasonable ways to allow the `blogc` user to push to the remote repository are:

- Create a password-less SSH key. The key *must* be password-less, because the push is automatic, and remote git hooks can't be interactive.
- Create an oauth token in the hosting service (e.g. GitHub or Bitbucket) and add it to the git URL in `.mirror`.

The mirroring feature wont't block a `git push`, it will just raise warnings. That means that if an error happens when mirroring the repository, your deploy will still succeed. Please pay attention to the git hook logs, to avoid losing data because your repositories are not being mirrored. ;)

----
If some unexpected error happened, please [file an issue](https://github.com/blogc/blogc-git-receiver/issues/new).

-- Rafael G. Martins <rafael@rafaelmartins.eng.br>
