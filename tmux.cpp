#include <iostream>

#include <KLocalizedString>
#include <KConfig>
#include <KConfigGroup>
#include <KTerminalLauncherJob>

#include <QApplication>
#include <QProcess>
#include <QIcon>

#include "tmux.h"

using namespace std;

Tmux::Tmux(QObject *parent, const KPluginMetaData &metadata)
    : KRunner::AbstractRunner(parent, metadata)
{
    // General runner configuration
    setObjectName(QLatin1String("Tmux"));
    addSyntax(
        KRunner::RunnerSyntax(
            QString::fromLatin1(":q:"),
            i18n("Opens a terminal window attached to a tmux session")
        )
    );
}

Tmux::~Tmux() {}

void Tmux::match(KRunner::RunnerContext &context)
{
    if (!context.isValid()) return;

    QString query = context.query();
    bool isQueryPrefixed = false;
    if (query.startsWith(QString::fromUtf8("tmux"))) {
	    query = query.right(query.length() - 4).trimmed();
	    isQueryPrefixed = true;
    }

    QList<KRunner::QueryMatch> matches;

    QProcess ls;
    ls.start(QString::fromUtf8("tmux"), QStringList() << QString::fromUtf8("ls"));
    ls.waitForFinished();
    bool matched = false;

    while (ls.canReadLine()) {
        QString line = QString::fromLocal8Bit(ls.readLine());
        QString sessionName = line.split(QChar::fromLatin1(':')).first();
        
        if (sessionName.startsWith(query)) {
            // We have a match
            matched = true;
            KRunner::QueryMatch match(this);

            // Basic properties for the match
            QMap<QString, QVariant> matchData;
            matchData.insert(QString::fromUtf8("sessionName"), sessionName);
            matchData.insert(QString::fromUtf8("exists"), true);

            match.setIcon(QIcon::fromTheme(QString::fromUtf8("utilities-terminal")));
            match.setText(sessionName);
            match.setSubtext(i18n("Attach to tmux session"));
            match.setData(matchData);
            match.setId(sessionName);

            float relevance = (float) query.length() / (float) sessionName.length();
            match.setRelevance(isQueryPrefixed ? 1.0 : relevance);

            matches.append(match);
        }
    }

    if (isQueryPrefixed && !matched) {
        KRunner::QueryMatch match(this);

        QMap<QString, QVariant> matchData;
        matchData.insert(QString::fromUtf8("sessionName"), query);
        matchData.insert(QString::fromUtf8("exists"), false);

        match.setIcon(QIcon::fromTheme(QString::fromUtf8("utilities-terminal")));
        match.setText(query);
        match.setSubtext(i18n("Create tmux session"));
        match.setId(query);
        match.setRelevance(1.0);
        match.setData(matchData);
        matches.append(match);
    }

    context.addMatches(matches);
}

void Tmux::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match)
{
    Q_UNUSED(context);
    QMap<QString, QVariant> matchData = match.data().toMap();

    QStringList args;
    args.append(QString::fromUtf8("tmux"));

    if (matchData.value(QString::fromUtf8("exists")).toBool()) {
        args.append(QString::fromUtf8("attach"));
        args.append(QString::fromUtf8("-t"));
    } else {
        args.append(QString::fromUtf8("new"));
        args.append(QString::fromUtf8("-c"));
        args.append(QString::fromUtf8("~"));
        args.append(QString::fromUtf8("-s"));
    }

    args.append(matchData.value(QString::fromUtf8("sessionName")).toString());

    KTerminalLauncherJob job(args.join(QString::fromUtf8(" ")));
    job.start();
}

K_PLUGIN_CLASS_WITH_JSON(Tmux, "tmux.json")

#include "tmux.moc"
