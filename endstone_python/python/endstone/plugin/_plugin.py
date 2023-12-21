from endstone._api import Plugin as PluginBase


class Plugin(PluginBase):
    def __init__(self):
        PluginBase.__init__(self)

    @property
    def __description__(self):
        # TODO: pass description from python back to c++, should not be called by end users
        raise NotImplementedError
