#ifndef command_h
#define command_h

#include<memory>
#include<string>

class Command {
public:
    int id = -1;

    Command()                           {}
    explicit Command(const int& _id) : id(_id)   {}
};

class MoveCommand : public Command
{
public:
    int to = -1;

    explicit MoveCommand(const int& _id, const int& _to) : Command::Command(_id), to(_to) {}
};

#endif // command_h
