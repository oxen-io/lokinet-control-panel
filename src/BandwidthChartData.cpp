#include "BandwidthChartData.hpp"

#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickItem>
#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

constexpr uint64_t NUM_SAMPLES = 128;

BandwidthChartData::BandwidthChartData(QObject *parent)
    : QObject(parent)
    , m_numSamples(NUM_SAMPLES)
{
    m_uploadSamples.reserve(m_numSamples);
    m_downloadSamples.reserve(m_numSamples);

    // 0-fill samples
    for (uint64_t i=0; i<m_numSamples; ++i) {
        QPointF point(i, 0);
        m_uploadSamples.append(point);
        m_downloadSamples.append(point);
    }
}

static void appendSample(QVector<QPointF>& samples, uint64_t maxSamples, qreal sample) {
    if ((uint64_t)samples.size() >= maxSamples) {
        // shift entire dataset, discarding first (oldest), and append new sample
        for (uint64_t i=0; i<maxSamples-1; ++i) {
            // TODO: use timestamps instead of indices
            auto next = samples[i+1];
            samples[i].setY(next.y());
        }
        samples[maxSamples-1].setY(sample);
    } else {
        QPointF point(samples.size(), sample);
        samples.append(point);
    }
}

void BandwidthChartData::addUploadSample(qreal sample) {
    appendSample(m_uploadSamples, m_numSamples, sample);
}

void BandwidthChartData::addDownloadSample(qreal sample) {
    appendSample(m_downloadSamples, m_numSamples, sample);
}

void BandwidthChartData::updateUploadSeries(QtCharts::QAbstractSeries *series) {
    assert(series != nullptr);
    QtCharts::QXYSeries *xySeries = static_cast<QtCharts::QXYSeries *>(series);
    xySeries->replace(m_uploadSamples);
}

void BandwidthChartData::updateDownloadSeries(QtCharts::QAbstractSeries *series) {
    assert(series != nullptr);
    QtCharts::QXYSeries *xySeries = static_cast<QtCharts::QXYSeries *>(series);
    xySeries->replace(m_downloadSamples);
}

