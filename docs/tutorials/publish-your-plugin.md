# Publish your plugin

Let's say you have finished all the work on the new release of your project, written the release notes,
increased the version number, tagged the release and are ready to publish.

## Install `twine`

You will need to install [Twine] if you haven't done so.
[Twine] is a utility for publishing Python packages to PyPI and other repositories.

```shell
pip install twine
```

## Build your package

Build your package is as simple as one line of command.

```shell
pipx run build
```

This will build the package in an isolated environment, generating a source-distribution and wheel in the
`dist/` directory.

## Upload to [TestPyPI]

Unlike [PyPI], which is the actual index of all python packages, [TestPyPI] is a separate instance of the Python
Package Index. It's a good place to try distribution tools and processes without affecting the real index.

Because [TestPyPI] has a separate database from the live [PyPI], you’ll need a separate user account specifically
for TestPyPI. Go to https://test.pypi.org/account/register/ to register your account.

Once done, you can upload your distributions to [TestPyPI] using twine by specifying the `--repository`/`-r` flag:

```shell
twine upload -r testpypi dist/*
```

Twine will prompt for your **API token** or username and password.

!!! tip
For security reasons it is strongly recommended to **create an API token** instead of using your username and
password when uploading a package to PyPI. If you haven’t done so, create an API token on both
[PyPI](https://pypi.org/manage/account/token/) and the [TestPyPI](https://test.pypi.org/manage/account/token/).
You will also be asked to choose the scope of this token, for now you can just leave the token unrestricted.

## Upload to [PyPI]

Now if things looks right, you can upload to the actual Python Package Index - [PyPI]:

```shell
twine upload -r pypi dist/*
```

## Publish using GitHub Actions CI/CD workflows

GitHub Actions CI/CD allows you to run a series of commands whenever an event occurs on the GitHub platform.
This can be a great choice if you want your plugin be published to PyPI automatically whenever you created a release.

For more information, please follow
the [instructions](https://packaging.python.org/en/latest/guides/publishing-package-distribution-releases-using-github-actions-ci-cd-workflows/)
here.

## And that's it!

**:partying_face: Congratulations!** If everything goes well, you should soon be able to see your plugin on
`https://pypi.org/project/<package name>`.


[Twine]: https://twine.readthedocs.io/

[PyPI]: https://pypi.org/

[TestPyPI]: https://test.pypi.org/
