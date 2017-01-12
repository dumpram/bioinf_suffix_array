import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SuffixArrayConstruction {

	static boolean DEBUG = false;
	static boolean TEST = false;

	public static boolean LTYPE = false;
	public static boolean STYPE = true;
	public static boolean BUCKET_END = true;
	public static boolean BUCKET_START = false;

	static int d = 2;
	
	
	public static void main(String[] args) throws IOException {
		Runtime r = Runtime.getRuntime();
		
		d = Integer.parseInt(args[0]);
		String in = args[1];
		String out = args[2];
		
		List<String> lines = Files.readAllLines(Paths.get(in));
		List<Integer> array = new ArrayList<Integer>();
		
		for (String line : lines) {
				if (line.startsWith(">")) {
					continue;
				}
				for (char i : line.toCharArray()) {
					array.add((int)i);
				}
		}
		array.add(0);
		
		int[] SA = new int[array.size()];
		int[] s = new int[array.size()];
		for (int i = 0; i < array.size(); i++) {
			s[i] = array.get(i);
		}
		lines = null;
		array = null;
		System.gc();
		
		/**
		 * Call SADS and record time and memory consumption.
		 */
		long memoryBefore = r.totalMemory() - r.freeMemory();
		long time = System.nanoTime();
		
		SA_DS(s, SA, 256, 0);
		time = System.nanoTime() - time;
		long memoryAfter = r.totalMemory() - r.freeMemory();
		
		System.out.println(isSorted(SA, s, SA.length));
		
		System.out.println((int)(time / 1e6) + " " + (memoryAfter - memoryBefore));
		
		
		List<String> outLines = new ArrayList<String>(SA.length);
		for (int i = 0; i < SA.length; i++) {
			outLines.add(Integer.toString(SA[i]));
		}
		Files.write(Paths.get(out), outLines);
	}

	public static void SA_DS(int[] s, int[] SA, int alphabetSize, int recursionLevel) {
		boolean[] t = new boolean[s.length];
		mapLSType(s, t);
		int[] P1 = new int[s.length];
		int n1 = getDCriticalPointers(s, t, P1);

		int[] b = new int[n1];
		int[] a = new int[n1];

		int[] sw = new int[s.length];
		for (int i = 0; i < sw.length; i++) {
			if (t[i]) {
				sw[i] = (2 * s[i] + 1);
			} else {
				sw[i] = (2 * s[i]);
			}
		}

		int[] P_1 = new int[n1];
		for (int i = 0; i < n1; i++) {
			P_1[i] = P1[i];
		}

		if (DEBUG)
			logDebugLine("Sw: " + Arrays.toString(sw));

		if (DEBUG)
			logDebugLine("P1: " + Arrays.toString(P1));
		bucketSortLS(P1, a, s, t, n1, d + 1);
		//a = P1;
		for (int i = 0; i < d + 2; i++) {
			if (i % 2 == 0) {
				bucketSort(a, b, s, n1, alphabetSize, d + 1 - i);
				P1 = b;
			} else {
				bucketSort(b, a, s, n1, alphabetSize, d + 1 - i);
				P1 = a;
			}
			if (DEBUG)
				logDebugLine("Sort " + (i + 1) + ":" + Arrays.toString(P1));
		}
		if (DEBUG)
			logDebugLine("Complete pass: " + Arrays.toString(P1));
		for (int i = 0; i < t.length; i++) {
			if (t[i] == LTYPE) {
				logDebug("0 ");
			} else {
				logDebug("1 ");
			}
		}
		logDebugLine("");
		int name = 0;

		int[] S1 = new int[n1];
		boolean diff = false;
		for (int i = 0; i < n1; i++) {
			if (i == 0) {
				S1[0] = name;
				continue;
			}
			diff = false;
			for (int j = 0; j < d + 2; j++) {
				if (sw[P1[i - 1] + j] != sw[P1[i] + j]) {
					diff = true;
					S1[i] = ++name;
					break;
				}
			}
			if (!diff) {
				S1[i] = name;
			}
		}

		int[] tmp = new int[s.length];
		for (int i = 0; i < tmp.length; i++) {
			tmp[i] = -1;
		}
		for (int i = 0; i < n1; i++) {
			tmp[P1[i]] = S1[i];
		}
		for (int i = 0, k = 0; i < s.length; i++) {
			if (tmp[i] != -1) {
				S1[k++] = tmp[i];
			}
		}
		if (DEBUG)
			logDebugLine("S1: " + Arrays.toString(S1));

		int[] SA1 = new int[S1.length];

		if (name < n1 - 1) {
			SA_DS(S1, SA1, name + 1, recursionLevel + 1);
			System.gc();
		} else {
			SA1 = new int[S1.length];
			// induce SA1 from S1
			logDebugLine("Inducing SA1 from S1");

			for (int i = 0; i < SA1.length; i++) {
				SA1[S1[i]] = i;
			}
			if (DEBUG)
				logDebugLine("SA1: " + Arrays.toString(SA1));
		}
		// induce SA from SA1
		if (DEBUG)
			logDebugLine("Inducing SA from SA1: " + Arrays.toString(SA1));

		for (int i = 0; i < SA.length; i++) {
			SA[i] = -1;
		}
		int[] B = getBuckets(s, alphabetSize, BUCKET_END);

		for (int i = SA1.length - 1; i >= 0; i--) {
			// check if is LMS and add to end of the bucket
			if (t[P_1[SA1[i]]] == STYPE && t[P_1[SA1[i]] - 1] == LTYPE) {
				SA[--B[s[P_1[SA1[i]]]]] = P_1[SA1[i]];
			}
		}
		if (DEBUG)
			logDebugLine("Step 1: " + Arrays.toString(SA));

		B = getBuckets(s, alphabetSize, BUCKET_START);
		for (int i = 0; i < SA.length; i++) {
			if (SA[i] > 0) {
				if (t[SA[i] - 1] == LTYPE) {
					SA[B[s[SA[i] - 1]]++] = SA[i] - 1;
				}
			}
		}
		if (DEBUG)
			logDebugLine("Step 2: " + Arrays.toString(SA));

		B = getBuckets(s, alphabetSize, BUCKET_END);
		for (int i = SA.length - 1; i >= 0; i--) {
			if (SA[i] > 0) {
				if (t[SA[i] - 1] == STYPE) {
					SA[--B[s[SA[i] - 1]]] = SA[i] - 1;
				}
			}
		}
		if (DEBUG)
			logDebugLine("Step 3: " + Arrays.toString(SA));
	}

	public static int[] getBuckets(int[] s, int alphabetSize, boolean getEnds) {
		int[] bucket = new int[alphabetSize];
		for (int i = 0; i < s.length; i++) {
			bucket[s[i]]++;
		}
		int sum = 0;
		for (int i = 0; i < alphabetSize; i++) {
			sum += bucket[i];
			bucket[i] = getEnds ? sum : sum - bucket[i];
		}
		return bucket;
	}

	public static int getDCriticalPointers(int[] s, boolean[] t, int[] P1) {
		int k = 0;
		for (int i = 1; i < s.length; i++) {
			if (t[i] == STYPE && t[i - 1] == LTYPE) {
				P1[k++] = i;
			} else if (!(t[i + 1] == STYPE && t[i] == LTYPE)) {
				if (k > 0 && P1[k - 1] == i - d) {
					P1[k++] = i;
				}
			}
		}
		return k;
	}

	public static void mapLSType(int[] s, boolean[] t) {
		for (int i = s.length - 1; i >= 0; i--) {
			if (i == s.length - 1) {
				t[i] = STYPE;
			} else if (s[i] > s[i + 1] || s[i] == s[i + 1] && t[i + 1] == LTYPE) {
				t[i] = LTYPE;
			} else {
				t[i] = STYPE;
			}
		}
	}

	public static void bucketSort(int[] a, int[] b, int[] s, int n1, int alphabetSize, int d) {

		int[] c = new int[alphabetSize];

		for (int i = 0, j = 0; i < n1; i++) {
			j = a[i] + d;
			j = (j > s.length - 1) ? s.length - 1 : j;
			c[s[j]]++;
		}

		for (int i = 0, sum = 0; i < alphabetSize; i++) {
			int t = c[i];
			c[i] = sum;
			sum += t;
		}

		for (int i = 0, j = 0; i < n1; i++) {
			j = a[i] + d;
			j = (j > s.length - 1) ? s.length - 1 : j;
			b[c[s[j]]++] = a[i];
		}
	}

	public static void bucketSortLS(int[] a, int[] b, int[] s, boolean[] t, int n1, int d) {

		int[] c = { 0, n1 - 1 };

		for (int i = 0, j = 0; i < n1; i++) {
			j = a[i] + d;
			j = (j > s.length - 1) ? s.length - 1 : j;
			if (t[j] == STYPE) {
				b[c[1]--] = a[i];
			} else {
				b[c[0]++] = a[i];
			}
		}
	}

	public static void logDebugLine(String s) {
		if (DEBUG) {
			System.out.println(s);
		}
	}

	public static void logDebug(String s) {
		if (DEBUG) {
			System.out.print(s);
		}
	}

	static int sless(int[] s, int p1, int p2, int n) {
		for (int i = 0; i < n; i++) {
			if (s[p1 + i] < s[p2 + i])
				return 1;
			if (s[p1 + i] > s[p2 + i])
				return -1;
		}
		return 0;
	}

	// test if SA is sorted for the input string s
	static boolean isSorted(int[] SA, int[] s, int n) {
		for (int i = 0; i < n - 1; i++) {
			int d = SA[i] < SA[i + 1] ? n - SA[i + 1] : n - SA[i];
			int rel = sless(s, SA[i], SA[i + 1], d);
			if (rel == -1 || rel == 0 && SA[i + 1] > SA[i])
				return false;
		}
		return true;
	}

}
