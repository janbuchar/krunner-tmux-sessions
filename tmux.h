#ifndef TMUX_H
#define TMUX_H

#include <KRunner/AbstractRunner>

class Tmux : public Plasma::AbstractRunner
{
    Q_OBJECT

public:
    Tmux(QObject *parent, const KPluginMetaData &metadata, const QVariantList &args);
    ~Tmux();

    void match(Plasma::RunnerContext &) override;
    void run(const Plasma::RunnerContext &, const Plasma::QueryMatch &) override;
};

#endif
