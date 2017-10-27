#ifndef TMUX_H
#define TMUX_H

#include <KRunner/AbstractRunner>

class Tmux : public Plasma::AbstractRunner
{
    Q_OBJECT

public:
    Tmux(QObject *parent, const QVariantList &args);
    ~Tmux();

    void match(Plasma::RunnerContext &);
    void run(const Plasma::RunnerContext &, const Plasma::QueryMatch &);
};

#endif
