#ifndef DEBUGHELPER_H
#define DEBUGHELPER_H

#ifdef QT_DEBUG

const int debugLevel = 1;

#define DEBUG(l, x) if (debugLevel >= l) qDebug() << x;
#define DEBUG_TAG(l, tag, x) if (debugLevel >= l) qDebug() << "[" << tag << "]" << x;
#define WARNING(l, x) if (debugLevel >= l) qWarning() << x;
#define WARNING_TAG(l, tag, x) if (debugLevel >= l) qWarning() << "[" << tag << "]" << x;

#endif

#endif // DEBUGHELPER_H
