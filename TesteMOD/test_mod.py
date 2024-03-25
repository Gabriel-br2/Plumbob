import sims4.commands

@sims4.commands.Command('hellow', command_type=sims4.commands.CommandType.Live)
def sayhello(_connection=None):
    output = sims4.commands.CheatOutput(_connection)
    output('Hello World')