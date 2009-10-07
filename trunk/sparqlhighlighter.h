#ifndef SPARQLHIGHLIGHTER_H
#define SPARQLHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class QTextEdit;


class SparqlHighlighter : public QSyntaxHighlighter
{
public:
    SparqlHighlighter( QTextEdit * parent );\
    void highlightBlock(const QString &text);
};

#endif // SPARQLHIGHLIGHTER_H
