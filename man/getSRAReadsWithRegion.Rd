% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/RcppExports.R
\name{getSRAReadsWithRegion}
\alias{getSRAReadsWithRegion}
\title{The reads in the specified region in an SRA record.}
\usage{
getSRAReadsWithRegion(acc, refname, start, stop)
}
\arguments{
\item{acc}{An accession or a path to an actual SRA file (with .sra suffix)}

\item{start}{Start position (inclusive)}

\item{stop}{End position (inclusive)}

\item{ref}{The reference name}
}
\value{
the reads in the collection
}
\description{
This returns the all reads in the specified region.
}
\examples{
getSRAReadsWithRegion('SRR789392','NC_000020.10', 62926240, 62958722)
}

