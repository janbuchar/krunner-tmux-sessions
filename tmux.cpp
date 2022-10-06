#include <iostream>

#include <KLocalizedString>
#include <KConfigCore/KConfig>
#include <KConfigCore/KConfigGroup>
#include <KTerminalLauncherJob>

#include <QApplication>
#include <QProcess>

#include "tmux.h"

using namespace std;

Tmux::Tmux(QObject *parent, const KPluginMetaData &metadata, const QVariantList &args)
    : Plasma::AbstractRunner(parent, metadata, args)
{
    Q_UNUSED(args);
    
    // General runner configuration
    setObjectName(QLatin1String("Tmux"));
    setPriority(HighestPriority);
    addSyntax(
        Plasma::RunnerSyntax(
            QString::fromLatin1(":q:"),
            i18n("Opens a terminal window attached to a tmux session")
        )
    );
}

Tmux::~Tmux() {}

void Tmux::match(Plasma::RunnerContext &context)
{
    if (!context.isValid()) return;

    QString query = context.query();
    bool isQueryPrefixed = false;
    if (query.startsWith("tmux")) {
	    query = query.right(query.length() - 4).trimmed();
	    isQueryPrefixed = true;
    }

    QList<Plasma::QueryMatch> matches;

    QProcess ls;
    ls.start("tmux", QStringList() << "ls");
    ls.waitForFinished();
    bool matched = false;

    while (ls.canReadLine()) {
        QString line = QString::fromLocal8Bit(ls.readLine());
        QString sessionName = line.split(':').first();
        
        if (sessionName.startsWith(query)) {
            // We have a match
	    matched = true;
            Plasma::QueryMatch match(this);
            // Basic properties for the match
	    QMap<QString, QVariant> matchData;
	    matchData.insert("sessionName", sessionName);
	    matchData.insert("exists", true);
            match.setIcon(QIcon::fromTheme("utilities-terminal"));
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
        Plasma::QueryMatch match(this);

	QMap<QString, QVariant> matchData;
	matchData.insert("sessionName", query);
	matchData.insert("exists", false);

        match.setIcon(QIcon::fromTheme("utilities-terminal"));
        match.setText(query);
        match.setSubtext(i18n("Create tmux session"));
        match.setId(query);
        match.setRelevance(1.0);
	match.setData(matchData);
        matches.append(match);
    }

    context.addMatches(matches);
}

void Tmux::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context);
    QMap<QString, QVariant> matchData = match.data().toMap();

    QStringList args;
    args.append("tmux");

    if (matchData.value("exists").toBool()) {
        args.append("attach");
        args.append("-t");
    } else {
	args.append("new");
        args.append("-c");
        args.append("~");
        args.append("-s");
    }

    args.append(matchData.value("sessionName").toString());

    KTerminalLauncherJob job(args.join(" "));
    job.start();
}

K_EXPORT_PLASMA_RUNNER(tmux, Tmux)

#include "tmux.moc"
