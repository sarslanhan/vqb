#include "sparqlhighlighter.h"

#include <QTextEdit>

SparqlHighlighter::SparqlHighlighter(QTextEdit * parent)
        : QSyntaxHighlighter(parent)
{
}

void SparqlHighlighter::highlightBlock(const QString &text)
{
    //keywords
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);
    QString pattern = "\\b(SELECT|DISTINCT|WHERE|FILTER|PREFIX|CONSTRUCT|a)\\b";

    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
    }

    //strings and values
    myClassFormat.setFontWeight(QFont::Normal);
    myClassFormat.setForeground(Qt::darkGreen);
    pattern = "(\".*\")|('.*')|(\\d+)";;

    expression.setPattern(pattern);
    index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
    }

    //uris
    myClassFormat.setFontWeight(QFont::Normal);
    myClassFormat.setForeground(Qt::darkBlue);
    pattern = "\\b((<[\\S\\s]*>)|(\\S+:\\S+))\\b";

    expression.setPattern(pattern);
    index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
    }

    //vars
    //FIXME: this doesn't work, if the var is at the beginning of the row
    myClassFormat.setFontWeight(QFont::Normal);
    myClassFormat.setForeground(Qt::red);
    pattern = "(\\?|\\$)[A-Za-z]\\w*";

    expression.setPattern(pattern);
    index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
    }


}
