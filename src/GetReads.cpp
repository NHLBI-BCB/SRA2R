#include <Rcpp.h>
#include <ncbi-vdb/NGS.hpp>
#include <ngs-bam/ngs-bam.hpp>
#include <ngs/ErrorMsg.hpp>
#include <ngs/ReadCollection.hpp>
#include <ngs/ReadIterator.hpp>
#include <ngs/Read.hpp>


#include <math.h>
#include <iostream>

using namespace ngs;
using namespace std;
using namespace Rcpp;



//' The readCount in the read collection.
//'
//' This simply returns the full read count.
//'
//' @param acc An accession or a path to an actual SRA file (with .sra suffix)
//' @return the number of reads in the collection
//' @export
//' @examples
//' getFastqCount('SRR000123')
// [[Rcpp::export]]
long getFastqCount(Rcpp::String acc) {
  try {
    ReadCollection run = ncbi::NGS::openReadCollection ( acc );
    long MAX_ROW = run.getReadCount ();
    return MAX_ROW;
  } catch(std::exception &ex) {	
    forward_exception_to_r(ex);
    return -1;
  } catch(...) { 
    ::Rf_error("c++ exception (unknown reason)"); 
    return -1;
  }
}


//' The reads in the read collection.
//'
//' This returns the all reads.
//'
//' @param acc An accession or a path to an actual SRA file (with .sra suffix)
//' @param n The number of reads to return
//' @return the reads in the collection
//' @export
//' @examples
//' getFastqReads('SRR000123')
// [[Rcpp::export]]
Rcpp::List getFastqReads(Rcpp::String acc) {
  try {
    ReadCollection run = ncbi::NGS::openReadCollection ( acc );
    long MAX_ROW = run.getReadCount (); 
    
    ReadIterator rgi = run.getReads( Read::all );
    
    vector<std::string> out;
    for(int i = 0; rgi.nextRead() ; i++) {
      while ( rgi.nextFragment() ) {
        out.push_back(rgi.getFragmentBases().toString());
      }
    }
    return List::create (
        _["reads"] = out
    );
  } catch(std::exception &ex) {	
    forward_exception_to_r(ex);
    return -1;
  } catch(...) { 
    ::Rf_error("c++ exception (unknown reason)"); 
    return -1;
  }
}

//' The reads in the read collection.
//'
//' This returns the all reads.
//'
//' @param acc An accession or a path to an actual SRA file (with .sra suffix)
//' @param n The number of reads to return
//' @return the reads in the collection
//' @export
//' @examples
//' getFastqReadsWithQuality('SRR000123')
// [[Rcpp::export]]
Rcpp::List getFastqReadsWithQuality(Rcpp::String acc) {
  try {
    ReadCollection run = ncbi::NGS::openReadCollection ( acc );
    long MAX_ROW = run.getReadCount (); 
    
    ReadIterator rgi = run.getReads( Read::all );
    CharacterVector reads(MAX_ROW);
    CharacterVector qualities(MAX_ROW);

    for(int i = 0; rgi.nextRead() && ( i < MAX_ROW ) ; i++) {
      while ( rgi.nextFragment() ) {
        reads[i] = rgi.getFragmentBases().toString();
        qualities[i] = rgi.getFragmentQualities().toString();
      }
    }
    
    List fastqRead = Rcpp::List::create(Rcpp::Named("read") = reads,
                                        Rcpp::Named("quality") = qualities);
    return fastqRead;
  } catch(std::exception &ex) {	
    forward_exception_to_r(ex);
    return -1;
  } catch(...) { 
    ::Rf_error("c++ exception (unknown reason)"); 
    return -1;
  }
}

//' The reads in the specified region.
//'
//' This returns the all reads in the specified region.
//'
//' @param acc An accession or a path to an actual SRA file (with .sra suffix)
//' @param ref The reference name 
//' @param start Start position (inclusive)
//' @param stop End position (inclusive)
//' @return the reads in the collection
//' @export
//' @examples
//' getFastqReadsWithRegion('SRR789392','NC_000020.10', 62926240, 62958722)
// [[Rcpp::export]]
Rcpp::List getFastqReadsWithRegion(Rcpp::String acc, Rcpp::String refname, long start, long stop) {
  try {
    ReadCollection run = ncbi::NGS::openReadCollection ( acc );
  
    //testing whether there is alignment
    try {
      long alignmentCount = run.getAlignmentCount();
      if (alignmentCount==0) {
        throw std::range_error("no aligned reads availabe"); 
        return(Rcpp::List());
      }
    } catch (ngs::ErrorMsg ngsErr){
      forward_exception_to_r(ngsErr);
      return(Rcpp::List());
    }
    
    try {
      // get requested reference
      ngs::Reference ref = run.getReference ( refname );
      
      long referenceLength = ref.getLength();
      if (stop<start || stop>referenceLength || start<1) {
        throw std::range_error("wrong reference range, reference length = " + toString(referenceLength)); 
        return(Rcpp::List());
      } 
      
      long count = stop - start + 1;
      AlignmentIterator alignit = ref.getAlignmentSlice ( start, count, Alignment::primaryAlignment );
      vector<std::string> reads;
      vector<std::string> qualities;
      
      while( alignit.nextAlignment() ) {
        reads.push_back(alignit.getFragmentBases().toString());
      }
      return List::create (
          _["reads"] = reads
      );
      
    } catch (ngs::ErrorMsg ngsErr){
      forward_exception_to_r(ngsErr);
      return(Rcpp::List());
    }
  } catch(std::exception &ex) {	
    forward_exception_to_r(ex);
    return(Rcpp::List());
  } catch(...) { 
    ::Rf_error("c++ exception (unknown reason)"); 
    return(Rcpp::List());
  } //try ReadCollection run
  
}
