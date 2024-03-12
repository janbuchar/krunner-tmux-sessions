#ifndef TMUX_H
#define TMUX_H

#include <KRunner/AbstractRunner>

class Tmux : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    Tmux(QObject *parent, const KPluginMetaData &metadata);
    ~Tmux();

    void match(KRunner::RunnerContext &) override;
    void run(const KRunner::RunnerContext &, const KRunner::QueryMatch &) override;
};

#endif
